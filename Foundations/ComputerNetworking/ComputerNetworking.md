# Computer Networking
## `\n` and `\r`
1. `\r\n` is used to end an HTTP request
2. `\r` -> Carriage Return, jump to the beginning of line
3. `\n` -> Line Feed, jump to next line
## Byte Stream
1. each sender and receiver maintains a byte stream
### I. Sender byte stream
1. Sender maintains a byte stream, once acked, it moves forward
### II. Receiver byte stream
1. receiver splits the stream into -> assembled, unassembled
2. assembled means ready to be read, unassembled means lack of some bytes
3. `unread bytestream` + `window size` cannot be larger than capacity
4. window size can be controlled. e.g. upon successful transmit, size + 1, upon failure, size /= 2, it can use exponential grow to achieve the limit quickly.
## Sequence Number
### I. SYN, FIN
syn and fin has independent seqno
### II. data stream
1. each byte has a seqno (absolute)
2. because TCP only allows 32 bit seqno, which is too small if data stream is larger than 4GB. Sender needs to wrap 64bit absolute seqno into 32bit seqno and send. Receiver needs to unwrap 32bit seqno to 64bit seqno (closest to checkpoint, which is updated every time receiver receives a TCP segment), by closest I mean withi +-2^31
## Flow Control
### I. Sender
1. sender can use exponential back-off for flow-control purpose. Once failure, the RTO (Retransmission TimeOut) doubles. Once success, RTO recovers to its initial value.
### II. Receiver
1. window size control