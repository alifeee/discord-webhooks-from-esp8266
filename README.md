# Discord Webhooks from Microcontroller

Sending a Discord webhook (message to a channel) from an ESP8266 (D1 Mini).

![screenshot of Discord webhook message](images/webhook_example.png)

Test webhooks with a POST request

```bash
curl -X POST -H "Content-type: application/json" -d "@webhook_example.json" "${WEBHOOK_URL}"
```

The simplest webhook only needs a `content` field.

```json
{
    "content": "hello!"
}
```

To turn a JSON file into something you can put in `.cpp` files (i.e., escape the quotes), run something like:

```bash
cat webhook.json | jq -c | sed 's/"/\\"/g'
```

## Programming

### set up secrets

```bash
cp src/secrets.h.example src/secrets.h
nano src/secrets.h
```

Install <https://platformio.org/>, then:

### build code

```bash
pio run -e blink # blink (to test microcontroller)
pio run -e request # send request
```

### upload code

```bash
pio run -e blink -t upload # blink (to test microcontroller)
pio run -e request -t upload # send request
```

### attach to serial monitor

```bash
pio device monitor
```
