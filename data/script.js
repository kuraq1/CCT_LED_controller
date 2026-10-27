var sliderHot = document.getElementById("hotRange");
var sliderCold = document.getElementById("coldRange");

const statusHot = document.getElementById("hot-stat");
const statusCold = document.getElementById("cold-stat");

function change_slider_value(stat, url, new_value) {
    fetch(url + new_value)
        .then(response => {
            stat.innerText = new_value;
        })
        .catch(error => {
            console.log("Symulacja na PC: Wysłano " + adresURL);
            stat.innerText = new_value;
        });
}

sliderHot.oninput = function() {
    change_slider_value(statusHot, "/hot/", this.value);
};

sliderCold.oninput = function() {
    change_slider_value(statusCold, "/cold/", this.value);
};