document.addEventListener('DOMContentLoaded', () => {
    const coordinatesElement = document.getElementById('coordinates');

    // Example function to update coordinates
    function updateCoordinates(x, y) {
        coordinatesElement.textContent = `X: ${x}, Y: ${y}`;
    }

    // Call this function with the desired coordinates (for example)
    updateCoordinates(123, 456); // Example initial coordinates
});