// Function that runs when this script is loaded
function showParticleAlert(message) {
    alert("Particle says: " + message);
}

// Data received from Particle (from IFTTT Value1)
const particleData = {
    message: "{{Value1}}",
    timestamp: "{{OccurredAt}}"
};

// Function that button calls
function up() {
    showParticleAlert(particleData.message);
}
