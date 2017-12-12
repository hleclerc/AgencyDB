interface WeakSet<T> {
    add(value: T): WeakSet<T>;
    delete(value: T): boolean;
    has(value: T): boolean;
    // [Symbol.toStringTag]: "WeakSet";
}

interface WeakSetConstructor {
    new (): WeakSet<any>;
    new <T>(): WeakSet<T>;
    // new <T>(iterable: Iterable<T>): WeakSet<T>;
    prototype: WeakSet<any>;
}
declare var WeakSet: WeakSetConstructor;

export default WeakSet;
