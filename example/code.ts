function main() {
    console.log('Hello, ChromeDevTools!');
    console.log(`I heard you're an amazing tool! I'm here to play with you!`);

    for (let i = 1; i <= 5; ++i) {
        console.log(`${i} * ${i} = ${i * i}`);
    }

    const a = 7;
    const b = 5;
    const c = a * b;

    console.log(`${a} * ${b} = ${c}`);
}

const sayHi = (name: string) => {
    console.log(`Hi, ${name}`);
};

main();

sayHi('Foo');
