#!/bin/bash
locust -f locustfile.py --master --master-bind-host=127.0.0.1 --master-bind-port=5557  --web-port=7000