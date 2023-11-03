$(document).ready(function(){
    // Handle theme buttons
    $('.theme-button').click(function(){
        // get the selected theme from the button's data-theme attribute
        var theme = $(this).data('theme');
        
        // remove the current theme from the body
        $('body').removeClass();
        
        // apply the selected theme to the body
        $('body').addClass(theme);
        
        // save the selected theme to local storage
        localStorage.setItem('theme', theme);
    });
});