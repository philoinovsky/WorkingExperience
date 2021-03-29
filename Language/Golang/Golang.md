# Golang
## CSP
in `channel` model, workers don't communicate directly with each other, they communicate via `channel`, both sender and receiver are anonymous.
### Another model: Actor Model
actors commmunicates directly and asyncly.
1. all `actor` cannot be accessed by external, only accessable by its own internal thread or strand (inherently thread-safe)
2. only communicates by sending and receiving messages
3. `per-actor strand` to avoid concurrency to its member function.
4. `actor` may block itself, but wont block the thread