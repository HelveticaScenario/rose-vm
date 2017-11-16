var { RosebudVM, MEMSIZE } = require("bindings")("rosevm");
const keycodes = require("./keycodes");
RosebudVM.prototype.key.keycodes = keycodes;
RosebudVM.prototype.keyp.keycodes = keycodes;

module.exports = function(hd) {
  const buf = new Buffer(MEMSIZE);
  const vm = new RosebudVM(buf, hd);
  vm.screen = buf.slice(vm.screenOffset, vm.screenOffset + vm.screenLength);
  vm.palette = buf.slice(vm.paletteOffset, vm.paletteOffset + vm.paletteLength);
  // delete vm.screenOffset
  // delete vm.screenLength
  // delete vm.paletteOffset
  // delete vm.paletteLength
  Object.freeze(vm);
  return vm;
};
