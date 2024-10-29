FROM python:3.11-slim

WORKDIR /app

COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

COPY backend_client/app.py /app
COPY backend_client/run.py /app
COPY backend_client/config.py /app
COPY backend_client/mqtt_client.py /app

EXPOSE 5000

CMD ["python", "run.py"]
