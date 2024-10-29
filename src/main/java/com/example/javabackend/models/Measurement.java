package com.example.javabackend.models;

import com.fasterxml.jackson.annotation.JsonIgnore;
import jakarta.persistence.*;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.time.LocalDateTime;

@Entity
@Data
@NoArgsConstructor
public class Measurement {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    private float temperature;
    private float humidity;
    private LocalDateTime timestamp;

    @ManyToOne
    @JsonIgnore
    private Sensor sensor;

    public Measurement(Sensor sensor, float temperature, float humidity) {
        this.sensor = sensor;
        this.temperature = temperature;
        this.humidity = humidity;
        this.timestamp = LocalDateTime.now();
    }
}
