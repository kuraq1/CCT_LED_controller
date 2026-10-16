// 1. Odszukujemy elementy na stronie
const btnOn = document.getElementById("btn-on");
const btnOff = document.getElementById("btn-off");
const stanLed = document.getElementById("stan-led");

// 2. Funkcja do wysyłania komend
function wyslijKomende(adresURL, nowyTekst) {
    // Próbujemy połączyć się z adresem (np. /led/on)
    fetch(adresURL)
        .then(response => {
            // Jeśli ESP32 odebrało sygnał
            stanLed.innerText = nowyTekst;
        })
        .catch(error => {
            // Ponieważ testujesz na PC bez prawdziwego ESP32, fetch wyrzuci błąd.
            // Łapiemy ten błąd i symulujemy działanie, żebyś widział efekty podczas testów w VS Code.
            console.log("Symulacja na PC: Wysłano " + adresURL);
            stanLed.innerText = nowyTekst;
        });
}

// 3. Reakcja na kliknięcia
btnOn.addEventListener("click", () => wyslijKomende("/led/on", "Włączona"));
btnOff.addEventListener("click", () => wyslijKomende("/led/off", "Wyłączona"));