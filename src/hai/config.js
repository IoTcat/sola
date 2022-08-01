module.exports = {
    WARN_TIMES: 15,
    MQTT_SERVER: 'mqtt://192.168.3.6',
    ZONES: {
        liv: {
            in: {
                th: 60,
                n: 10,
                lock: 90,
                th_short: 40,
                n_short: 1,
                lock_short: 90
            },
            out: {
                th: 8,
                n: 5 * 60
            },
            frozen: 5 * 60 * 60
        },
        din: {
            in: {
                th: 60,
                n: 10,
                lock: 90,
                th_short: 40,
                n_short: 1,
                lock_short: 90
            },
            out: {
                th: 8,
                n: 3 * 60
            },
            frozen: 5 * 60 * 60
        },
        hall: {
            in: {
                th: 50,
                n: 2,
                lock: 180,
                th_short: 10,
                n_short: 1,
                lock_short: 30 //0
            },
            out: {
                th: 6,
                n: 3 * 60
            },
            frozen: 60 * 60
        }
    }
}