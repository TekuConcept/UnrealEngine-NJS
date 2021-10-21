var net = require('net')
var socket = net.createConnection(25562)

socket.on('connect', () => {
    process.stdin.resume()
    process.stdin.on('data', d => {
        socket.write(d)
    })
})
socket.pipe(process.stdout)
socket.on('end', done)
socket.on('close', done)

function done () {
    if (done.run) return
    done.run = true
    process.stdin.pause()
}
