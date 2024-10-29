FROM gradle:7.6.0-jdk17 AS build
WORKDIR /app
COPY ../.. .
RUN gradle bootJar -x test

# Runtime-steg
FROM eclipse-temurin:17-jdk-alpine
WORKDIR /app
COPY --from=build /app/build/libs/app.jar app.jar
EXPOSE 8080
ENTRYPOINT ["java", "-jar", "app.jar"]
