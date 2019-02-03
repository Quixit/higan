namespace higan::Object {

struct Port : Node {
  DeclareClass(Port, "Port")

  Port(string name = {}, string kind = {}) : Node(name), kind(kind) {
    allocate = [](auto) { return shared_pointer<Peripheral>::create(); };
  }

  auto connected() -> shared_pointer<Peripheral> {
    return find<shared_pointer<Peripheral>>(0);
  }

  auto connect(shared_pointer<Peripheral> peripheral) -> void {
    disconnect();
    prepend(peripheral);
    if(attach) attach(peripheral);
  }

  auto disconnect() -> void {
    if(auto peripheral = connected()) {
      if(detach) detach(peripheral);
      remove(peripheral);
    }
  }

  auto serialize(string& output, string depth) -> void override {
    Node::serialize(output, depth);
    if(kind) output.append(depth, "  kind: ", kind, "\n");
  }

  auto unserialize(Markup::Node node) -> void override {
    Node::unserialize(node);
    kind = node["kind"].text();
  }

  auto copy(shared_pointer<Node> node) -> void override {
    if(auto source = node->cast<shared_pointer<Port>>()) {
      kind = source->kind;
      if(auto peripheral = source->connected()) {
        auto node = allocate(peripheral->name);
        node->copy(peripheral);
        connect(node);
      }
    }
    Node::copy(node);
  }

  string kind;

  function<shared_pointer<Peripheral> (string)> allocate;
  function<void (shared_pointer<Peripheral>)> attach;
  function<void (shared_pointer<Peripheral>)> detach;
};

}