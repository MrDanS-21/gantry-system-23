$(document).ready(function(){
    if ($('.flash-message, .error-message-flash').length > 0) {
        showFlashMessage($('.flash-message, .error-message-flash').first());
    }

    // Check if there's a saved theme in local storage on page load
    var savedTheme = localStorage.getItem('theme');
    if (savedTheme) {
        // remove the current theme from the body
        $('body').removeClass();

        // apply the saved theme to the body
        $('body').addClass(savedTheme);

        // change the logo based on the theme
        updateLogo(savedTheme);
    }
});

function showFlashMessage(flash) {
    flash.delay(1000).slideDown().animate(
        { opacity: 0.9 },
        { queue: false, duration: 1000 }
    );
    
    setTimeout(function() {
        removeFlashMessage(flash);
    }, 10000);
    
    flash.click(function() {
        removeFlashMessage(flash);
    });
}

function removeFlashMessage(flash) {
    flash.slideUp(1000, function() {
        flash.remove();
        if ($('.flash-message, .error-message-flash').length > 0) {
            showFlashMessage($('.flash-message, .error-message-flash').first());
        }
    });
}

function updateLogo(theme) {
    var logoPath = "";
    if(theme === "dark-mode" || theme === "deep-ocean" || theme === "dark-plus" || theme === "twilight-mirage") {
        logoPath = "/static/LogoDark.png";
    } else {
        logoPath = "/static/LogoLight.png";
    }

    // Set the logo path
    $('#logo').attr("src", logoPath);
}