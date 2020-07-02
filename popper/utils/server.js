const net = require('net');
const port = 8080;
const host = '127.0.0.1';

const server = net.createServer();

server.on('connection', function (sock) {
  console.log('CONNECTED: ' + sock.remoteAddress + ':' + sock.remotePort);

  sock.on('data', function (data) {
    console.log('DATA ' + sock.remoteAddress + ': ' + data);
    const message = data.toString();

    if (message.search(/USER/ig) !== -1) {
      sock.write('+OK\n');
    }

    if (message.search(/PASS/ig) !== -1) {
      sock.write('+OK\n');
    }

    if (message.search(/LIST/ig) !== -1) {
      console.log("LIST command");

      sock.write(`+OK 1 215\n`);
    }

    if (message.search(/RETR/ig) !== -1) {
      console.log("RETR command");
      sock.write(`
+OK
Return-Path: <foo@example.org>
Delivered-To: fo@example.org
Date: ${new Date().toGMTString()}
From: Mario Rossi <mario@rossi.org>
Subject: xxxx
Content-Type: text/plain; charset=ISO-8859-1

this is the email body

.\n`);
    }

    if (message.search(/DELE/ig) !== -1) {
      sock.write('+OK\n');
    }
  });

  sock.on('close', function () {
    console.log('CLOSED: ' + sock.remoteAddress + ' ' + sock.remotePort);
  });

  sock.on('error', function (err) {
    console.log('ERROR: ' + err);
  });
});

server.listen(port, host, () => {
  console.log('TCP Server is running on port ' + port + '.');
});
