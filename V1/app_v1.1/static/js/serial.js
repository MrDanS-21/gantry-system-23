$(document).ready(function() {
    function fetchSerialData() {
        $.getJSON('/get_serial_data', function(data) {
            if (data && data.length > 0) {
                const currentContent = $('#serialOutput').val();
                $('#serialOutput').val(currentContent + "\n" + data.join("\n"));
    
                // Autoscroll
                const textarea = document.getElementById('serialOutput');
                textarea.scrollTop = textarea.scrollHeight;
            }
        });
    }
    
    function fetchServerData() {
        $.getJSON('/get_server_data', function(data) {
            if (data && data.length > 0) {
                const currentContent = $('#serverOutput').val();
                $('#serverOutput').val(currentContent + "\n" + data.join("\n"));
    
                // Autoscroll
                const textarea = document.getElementById('serverOutput');
                textarea.scrollTop = textarea.scrollHeight;
            }
        });
    }

    // Toggle between Arduino and Server Monitors
    $("#serverCheckbox").change(function() {
        if ($(this).prop("checked")) {
            $("#serverSection").show();
            $("#arduinoSection").removeClass("full-width");
        } else {
            $("#serverSection").hide();
            $("#arduinoSection").addClass("full-width");
        }
    });

    function sendSerialData(event) {
        // Prevent default form submission
        event.preventDefault();
    
        // Get the data from the input field
        const dataToSend = $('input[name="data_to_send"]').val();
    
        // Send data using AJAX
        $.ajax({
            url: "/send_serial_data",
            type: "POST",
            data: JSON.stringify({ 'data': dataToSend }),
            contentType: "application/json; charset=utf-8",
            dataType: "json",
            success: function(response) {
                $('input[name="data_to_send"]').val('');
                return true;
            },
            error: function(error) {
                return false;
            }
        });
    }
    
    // Attach the form submit event to sendSerialData function
    $("#sendDataForm").submit(sendSerialData);
    
    function sendServerData(event) {
        // Prevent default form submission
        event.preventDefault();
    
        // Get the data from the input field
        const dataToSend = $('input[name="data_to_server"]').val();
    
        // Send data using AJAX
        $.ajax({
            url: "/send_server_data",
            type: "POST",
            data: JSON.stringify({ 'data': dataToSend }),
            contentType: "application/json; charset=utf-8",
            dataType: "json",
            success: function(response) {
                $('input[name="data_to_server"]').val('');

                // Display server response in the serverOutput textarea
                const currentContent = $('#serverOutput').val();
                $('#serverOutput').val(currentContent + "\n" + response.message);

                // Autoscroll
                const textarea = document.getElementById('serverOutput');
                textarea.scrollTop = textarea.scrollHeight;

                return true;
            },
            error: function(error) {
                return false;
            }
        });
    }
    
    // Attach the form submit event to sendServerData function
    $("#sendServerDataForm").submit(sendServerData);

    // Poll for new serial data every second
    setInterval(fetchSerialData, 1000);
    setInterval(fetchServerData, 1000);
});