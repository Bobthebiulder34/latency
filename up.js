// Listen to Particle events (real-time)
function listenToParticle() {
    const deviceId = "0a10aced202194944a0540c4";   // your device ID
    const token = "53c84e0f44ea8e64df246fd365bb7e0dbb50ce46"; // your access token

    // Open a live connection to Particle's event stream
    const eventSource = new EventSource(
        `https://api.particle.io/v1/devices/events?access_token=${token}`
    );

    eventSource.onmessage = function(event) {
        const data = JSON.parse(event.data);
        console.log("Raw event:", data);
        alert("HII");
        // Only react if this came from *your* device
        if (data.coreid === deviceId) {
            // Show it on screen
            showParticleAlert(data.data);
        }
    };

    eventSource.onerror = function(err) {
        console.error("Event stream error:", err);
    };
}

// Show alert when event comes in
function showParticleAlert(message) {
    alert("New IFTTT upload: " + message);
}

// Start listening as soon as page loads
listenToParticle();
function up()
{
    alert("Patience");
}
