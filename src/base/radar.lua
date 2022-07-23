MQTT_PUBLISH_INTERVAL = 1000;
UART_CACHE = '';
uart.setup(0, 256000, 8, uart.PARITY_NONE, uart.STOPBITS_1, 0);
uart.on("data", string.char(245),function(data)
    if UART_CACHE ~= data then
        REG(MQTT_TOPIC_PREFIX..'snsr/radar', data);
        UART_CACHE = data;
    end
end, 0);