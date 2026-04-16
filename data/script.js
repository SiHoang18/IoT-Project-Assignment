function sendAction(deviceId, action) {
    const timestamp = new Date().getTime();
    const url = `/api/control?device=${deviceId}&action=${action}&t=${timestamp}`;
    
    console.log("Transmitting Action: ", url);
    
    fetch(url)
        .then(response => {
            if (!response.ok) {
                console.error("Action rejected by ESP32. HTTP Code: ", response.status);
            } else {
                console.log("Action confirmed by ESP32");
            }
        })
        .catch(error => {
            console.error("Network layer fetch error:", error);
        });
}

function updateData() {
    const timestamp = new Date().getTime();
    fetch(`/api/data?t=${timestamp}`)
        .then(response => {
            if (!response.ok) {
                throw new Error("Telemetry fetch failed");
            }
            return response.json();
        })
        .then(data => {
            document.getElementById('tempValue').innerText = data.temperature;
            document.getElementById('humValue').innerText = data.humidity;
            
            const apStatusElems = document.querySelectorAll('.AP_SSID');
            if (apStatusElems.length > 0 && apStatusElems[0].innerText.includes('[LOADING]')) {
                apStatusElems.forEach(el => el.innerText = "AP: " + data.ap_ssid);
            }
        })
        .catch(error => {
            console.error("Telemetry update error:", error);
        });
}

document.addEventListener('DOMContentLoaded', () => {
    updateData();
    setInterval(updateData, 3000);
});

function sendAction(deviceId, action) {
    const timestamp = new Date().getTime();
    fetch(`/api/control?device=${deviceId}&action=${action}&t=${timestamp}`).catch(e => console.error(e));
}

function submitConfig() {
    const formData = new URLSearchParams();
    formData.append('ssid', document.getElementById('cfg_ssid').value);
    formData.append('pass', document.getElementById('cfg_pass').value);
    formData.append('server', document.getElementById('cfg_server').value);
    formData.append('user', document.getElementById('cfg_user').value);
    formData.append('token', document.getElementById('cfg_token').value);

    fetch('/api/config', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: formData.toString()
    })
    .then(response => {
        if (response.ok) {
            alert("Configuration saved. System is rebooting to apply changes.");
        }
    })
    .catch(error => console.error("Config error:", error));
}