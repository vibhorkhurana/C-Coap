const coap = require('coap')
var req = coap.request('coap://localhost:5683/Temperature');
// var opts = {
//     host: 'localhost',
//     port: 5683,
//     method: 'GET',
//     confirmable: true,
//     observe: false,
//     pathname: '/Temperature'
// };

// var req = coap.request(opts)
req.on('response', function(res) {
    res.pipe(process.stdout)
    console.log(process.stdout)
    res.on('end', function() {
        process.exit(0)
    })
});
req.end