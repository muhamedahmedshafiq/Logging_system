#!/bin/bash
# Simple telemetry server using netcat (nc)
# Sends random float numbers via Unix Domain Socket

SOCKET_PATH="/tmp/telemetry.sock"

# Remove old socket if exists
rm -f "$SOCKET_PATH"

echo "✅ Starting simple telemetry server on $SOCKET_PATH"
echo "Sending float numbers every second..."

# Create named pipe
mkfifo /tmp/telemetry_pipe 2>/dev/null || true

# Start server that sends random floats
while true; do
    # Generate random float between 0-100
    FLOAT=$(awk -v min=0 -v max=100 'BEGIN{srand(); print min+rand()*(max-min)}')
    
    # Listen on socket and send data when client connects
    echo "$FLOAT" | nc -lU "$SOCKET_PATH" 2>/dev/null
    
    echo "📤 Sent: $FLOAT"
    sleep 1
done
