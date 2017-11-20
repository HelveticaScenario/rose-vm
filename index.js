var { RosebudVM, MEMSIZE } = require("bindings")("rosevm");
const {Keycodes, keycodeFromString, keycodeToString} = require("./keycodes");
RosebudVM.prototype.key.codes = Keycodes;
RosebudVM.prototype.keyp.codes = Keycodes;
RosebudVM.prototype.key.codeFromString = keycodeFromString;
RosebudVM.prototype.keyp.codeFromString = keycodeFromString;
RosebudVM.prototype.key.codeToString = keycodeToString;
RosebudVM.prototype.keyp.codeToString = keycodeToString;

module.exports = function(hd) {
  const buf = new Buffer(MEMSIZE);
  const vm = new RosebudVM(buf, hd);
  vm.screen = buf.slice(vm.screenOffset, vm.screenOffset + vm.screenLength);
  vm.palette = buf.slice(vm.paletteOffset, vm.paletteOffset + vm.paletteLength);
  vm.paletteMap = buf.slice(vm.paletteMapOffset, vm.paletteMapOffset + vm.paletteMapLength);
  // delete vm.screenOffset
  // delete vm.screenLength
  // delete vm.paletteOffset
  // delete vm.paletteLength
  Object.freeze(vm);
  return vm;
};
