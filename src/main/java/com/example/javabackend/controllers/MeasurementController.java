package com.example.javabackend.controllers;

import com.example.javabackend.exceptions.SensorNotFoundException;
import com.example.javabackend.DTOs.MeasurementDTO;
import com.example.javabackend.models.Measurement;
import com.example.javabackend.models.Sensor;
import com.example.javabackend.repositories.MeasurementRepo;
import com.example.javabackend.repositories.SensorRepo;
import com.example.javabackend.services.PythonClientService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.reactive.function.client.WebClientResponseException;

import java.util.HashMap;
import java.util.Map;

@RestController
@RequestMapping("/measurement")
public class MeasurementController {

    private final MeasurementRepo measurementRepo;
    private final SensorRepo sensorRepo;
    private final PythonClientService pythonClientService;

    @Autowired
    public MeasurementController(SensorRepo sensorRepo, MeasurementRepo measurementRepo, PythonClientService pythonClientService) {
        this.sensorRepo = sensorRepo;
        this.measurementRepo = measurementRepo;
        this.pythonClientService = pythonClientService;
    }

    @RequestMapping("/get/{name}")
    public ResponseEntity<Map<String, Object>> getCurrentMeasurement(@PathVariable String name) {
        Map<String, Object> responseMap = new HashMap<>();
        try {
            Sensor sensor = sensorRepo.findByName(name);
            if (sensor == null) {throw new SensorNotFoundException(name, "name");}

            MeasurementDTO data = pythonClientService.getMeasurement(sensor.getMacAddress());

            if (data == null) {
                responseMap.put("error", "Unexpected response format from Python client");
                return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body(responseMap);
            }

            Measurement measurement = new Measurement(sensor, data.getTemperature(), data.getHumidity());
            sensor.addMeasurement(measurement);
            measurementRepo.save(measurement);

            responseMap.put("sensor name", sensor.getName());
            responseMap.put("temperature", measurement.getTemperature());
            responseMap.put("humidity", measurement.getHumidity());
            responseMap.put("time", measurement.getTimestamp());

            System.out.println("Returning successful response");
            return ResponseEntity.ok(responseMap);

        } catch (WebClientResponseException e) {
            System.out.println("Caught WebClientResponseException: " + e.getStatusCode());
            if (e.getStatusCode() == HttpStatus.GATEWAY_TIMEOUT) {
                responseMap.put("error", "Timeout waiting for measurement data from Python client");
                responseMap.put("message", "The Python client did not respond in time.");
            } else {
                responseMap.put("error", "Failed to contact Python client");
            }
            return ResponseEntity.status(e.getStatusCode()).body(responseMap);
        } catch (Exception e) {
            e.printStackTrace();
            responseMap.put("error", "An unexpected error occurred");
            responseMap.put("details", e.getMessage());
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR).body(responseMap);
        }
    }

}
