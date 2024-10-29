package com.example.javabackend.controllers;

import com.example.javabackend.DTOs.SensorDTO;
import com.example.javabackend.exceptions.SensorNotFoundException;
import com.example.javabackend.models.Sensor;
import com.example.javabackend.repositories.SensorRepo;
import org.springframework.web.bind.annotation.*;
import java.util.List;

@RestController
@RequestMapping("/sensor")
public class SensorController {

    private final SensorRepo sensorRepo;
    public SensorController(SensorRepo sensorRepo) {this.sensorRepo = sensorRepo;}

    @PostMapping("/add")
    public void add(@RequestBody SensorDTO sensor){
        try { sensorRepo.save(new Sensor(sensor.getName(), sensor.getMacAddress())); }
        catch (Exception e) { e.printStackTrace(); }
    }

    //http://localhost:8080/sensor/getall
    @GetMapping("/getall")
    public List<Sensor> getAll() {
        return sensorRepo.findAll();
    }

    //http://localhost:8080/sensor/getbyname/vardagsrum
    @GetMapping("/getbyname/{name}")
    public Sensor getByName(@PathVariable String name) {
        Sensor sensor = sensorRepo.findByName(name);
        if (sensor == null) {throw new SensorNotFoundException(name, "name");}
        return sensorRepo.findByName(name);
    }
}
