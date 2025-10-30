# R-Type Monitoring Setup

This folder contains the monitoring setup for the R-Type server, including Prometheus, Grafana, and Node Exporter.  
Follow these instructions to run it locally and visualize metrics.

---

## 1. Folder Structure

monitoring/  
├── docker-compose.yml  
├── prometheus/  
│ └── prometheus.yml  
├── grafana/  
│ ├── grafana-data/  
│ └── node_exporter.json  

- `prometheus/` → Prometheus configuration
- `grafana/` → folder for Grafana related files
  - `grafana-data/` → persistent storage for Grafana data

---

## 2. Prerequisites

- Docker and Docker Compose installed
- Your R-Type server running and exposing metrics on `localhost:8080`


---

## 3. Running the Monitoring Stack

### 1. Go to the monitoring folder:

```bash
    cd monitoring
```

### 2. Start the monitoring stack using Docker Compose:

```bash
    docker-compose up -d
```

### 3. Verify that the containers are running:

```bash
    docker-compose ps
```

You should see three services: `prometheus`, `grafana`, and `node_exporter`.

| **Name**      | **Command**                 | **State** |
|---------------|-----------------------------|-----------|
| grafana       | /run.sh                     | Up        |
| node_exporter | /bin/node_exporter          | Up        |
| prometheus    | /bin/prometheus --config... | Up        |

If one of these services is not up, check the logs with:

```bash
    docker-compose logs <service_name>
```

You would likely have an issue with grafana on Linux with permissions, in that case run:

```bash
    sudo rm -rf grafana/grafana-data
    mkdir grafana/grafana-data
    sudo chown -R 472:472 grafana-data
```

### 4. Access Dashboards

   - Grafana → http://localhost:3000  
   Default credentials: admin / admin  
   Dashboards are automatically loaded via provisioning.(not the case for the moment)  

   - Prometheus → http://localhost:9090
   Verify that your targets are UP.

   - Node Exporter metrics → http://localhost:9100/metrics

### 5. Platform-Specific Notes

**Linux**

Docker may require proper permissions for mounted volumes:

mkdir -p grafana-dashboards
mkdir -p provisioning/dashboards
mkdir -p provisioning/datasources
chmod -R 777 grafana-dashboards provisioning


Use the host IP for Prometheus target instead of host.docker.internal if it is the case in the `docker-compose.yml` file:
    Replace host.docker.internal with your host IP, usually `172.17.0.1`

**macOS / Windows**
host.docker.internal works out-of-the-box for Prometheus targets.

No special volume permissions needed.

### 6. Restarting Services
```shell
   docker-compose down
   docker-compose up -d
```

From there you should be able to monitor your R-Type server metrics using Prometheus and Grafana!
Have fun creating dashboards and visualizing your data.