var repl = require('repl')
var net = require('net')

process.stdin.resume()

net.createServer(function (socket) {
    var repls = repl.start({
        prompt: "> ",
        input: socket,
        output: socket
    });
    // repls.context = global
    repls.on && repls.on('exit', () => {
        console.error('repl exit event, closing socket')
        socket.end()
    })
    socket.on('error', () => repls.close())
}).listen(25562)
