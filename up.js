// IMPORTANT: Do NOT hardcode access tokens in source. Load them from a secure
// location instead (environment variable, server-side proxy, or secret manager).
// Replace the placeholder below with your own secure retrieval method.
const PARTICLE_ACCESS_TOKEN = process?.env?.PARTICLE_ACCESS_TOKEN || "<REPLACE_WITH_YOUR_TOKEN_OR_FETCH_FROM_SERVER>";

// Connect to Particle Cloud event stream
const eventSource = new EventSource(
  `https://api.particle.io/v1/devices/events?access_token=${PARTICLE_ACCESS_TOKEN}`
);

// Listen for all events
eventSource.onmessage = function(event) {
  const data = JSON.parse(event.data);

  // Only handle the specific event you published from Particle
  if (data.event === "ifttt_upload") {
    const content = data.data; // Payload from Particle.publish()
    
    console.log("New Particle event:", content);

    // Update page content dynamically
    const display = document.getElementById("particle-output");
    if (display) {
      display.textContent = content;
    }

  // Alert example (optional)
  // alert("Particle says: " + content);
  }
};

// Handle connection errors
eventSource.onerror = function(err) {
  console.error("EventSource failed:", err);
};
