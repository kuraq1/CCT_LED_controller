// 1. Odszukujemy elementy na stronie
var sliderHot = document.getElementById("hotRange");
var sliderCold = document.getElementById("coldRange");

const statusHot = document.getElementById("hot-stat");
const statusCold = document.getElementById("cold-stat");

// 2. Funkcja do wysyłania komend
function change_slider_value(stat, url, new_value) {
    // Próbujemy połączyć się z adresem (np. /led/on)
    fetch(url + new_value)
        .then(response => {
            // Jeśli ESP32 odebrało sygnał
            stat.innerText = new_value;
        })
        .catch(error => {
            // Ponieważ testujesz na PC bez prawdziwego ESP32, fetch wyrzuci błąd.
            // Łapiemy ten błąd i symulujemy działanie, żebyś widział efekty podczas testów w VS Code.
            console.log("Symulacja na PC: Wysłano " + adresURL);
            stat.innerText = new_value;
        });
}

// 3. Reakcja na kliknięcia
sliderHot.oninput = function() {
    change_slider_value(statusHot, "/hot/", this.value);
};

sliderCold.oninput = function() {
    change_slider_value(statusCold, "/cold/", this.value);
};