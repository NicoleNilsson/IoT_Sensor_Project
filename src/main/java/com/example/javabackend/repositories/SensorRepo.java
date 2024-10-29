package com.example.javabackend.repositories;

import com.example.javabackend.models.Sensor;
import org.springframework.data.jpa.repository.JpaRepository;

public interface SensorRepo extends JpaRepository<Sensor, Long> {
    Sensor findByName(String name);
    Sensor findByMacAddress(String macAddress);
}
