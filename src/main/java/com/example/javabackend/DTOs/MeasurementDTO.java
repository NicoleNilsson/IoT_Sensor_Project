package com.example.javabackend.DTOs;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class MeasurementDTO {
    private float temperature;
    private float humidity;
    private String macAddress;
}