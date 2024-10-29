package com.example.javabackend.models;

import jakarta.persistence.*;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.ArrayList;
import java.util.List;

@Entity
//@Table(name = "sensor", uniqueConstraints = {
//        @UniqueConstraint(name = "UK_sensor_name", columnNames = "name"),
//        @UniqueConstraint(name = "UK_sensor_mac_address", columnNames = "mac_address")
//})
@Data
@NoArgsConstructor
public class Sensor {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column(name = "mac_address", nullable = false, unique = true)
    private String macAddress;

    @Column(name = "name", nullable = false, unique = true)
    private String name;

    @OneToMany(mappedBy = "sensor", cascade = CascadeType.ALL, orphanRemoval = true)
    private List<Measurement> measurements = new ArrayList<>();

    public Sensor(String name, String macAddress) {
        this.name = name;
        this.macAddress = macAddress;
    }

    public void addMeasurement(Measurement measurement) {
        measurement.setSensor(this);
        this.measurements.add(measurement);
    }
}

