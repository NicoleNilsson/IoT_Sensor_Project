package com.example.javabackend.services;

import com.example.javabackend.DTOs.MeasurementDTO;
import org.springframework.http.HttpStatus;
import org.springframework.http.HttpStatusCode;
import org.springframework.stereotype.Service;
import org.springframework.web.reactive.function.client.ClientResponse;
import org.springframework.web.reactive.function.client.WebClient;
import org.springframework.web.reactive.function.client.WebClientResponseException;

@Service
public class PythonClientService {

    private final WebClient webClient;

    public PythonClientService() {
        this.webClient = WebClient.create();
    }

    public MeasurementDTO getMeasurement(String macAddress) {
        String pythonClientUrl = "http://host.docker.internal:5000/request_measurement/" + macAddress;

        try {
            return webClient.get()
                    .uri(pythonClientUrl)
                    .retrieve()
                    .onStatus(HttpStatusCode::isError, ClientResponse::createException)
                    .bodyToMono(MeasurementDTO.class)
                    .block();

        } catch (WebClientResponseException e) {
            if (e.getStatusCode() == HttpStatus.GATEWAY_TIMEOUT) {
                System.out.println("504 Gateway Timeout from Python client for MAC: " + macAddress);
            }
            e.printStackTrace();
            throw e;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

}
