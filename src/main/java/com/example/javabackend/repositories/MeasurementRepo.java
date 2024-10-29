package com.example.javabackend.repositories;

import com.example.javabackend.models.Measurement;
import org.springframework.data.jpa.repository.JpaRepository;

public interface MeasurementRepo extends JpaRepository<Measurement, Long> {
}
