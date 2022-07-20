MQTT_PUBLISH_INTERVAL = 1000;
uart.setup(0, 256000, 8, uart.PARITY_NONE, uart.STOPBITS_1, 0);
uart.on("data", 100,function(data)
    REG(MQTT_TOPIC_PREFIX..'snsr/radar', data);
end, 0);