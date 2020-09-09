/* global fetchWASM */

let importObject = {
    imports: {}
};

try {
    const bytes = fetchWASM('../example/add.wasm');
    if (WebAssembly.validate(bytes)) {
        WebAssembly.compile(bytes).then(function (module) {
            WebAssembly
              .instantiate(module, importObject)
              .then(function(instance) {
                  console.log('result = ', instance.exports.add(4, 5));
              }).catch(function(err) {
                console.error(`instantiate error: ${err}`);
            });
        }).catch(function(err) {
            console.error(`module error: ${err}`);
        });
    }
} catch(err) {
    console.error(err);
}
