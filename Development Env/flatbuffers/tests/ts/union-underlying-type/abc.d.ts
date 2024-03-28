import { A } from './a.js';
import { B } from './b.js';
import { C } from './c.js';
export declare enum ABC {
    NONE = 0,
    A = 555,
    B = 666,
    C = 777
}
export declare function unionToAbc(type: ABC, accessor: (obj: A | B | C) => A | B | C | null): A | B | C | null;
export declare function unionListToAbc(type: ABC, accessor: (index: number, obj: A | B | C) => A | B | C | null, index: number): A | B | C | null;
