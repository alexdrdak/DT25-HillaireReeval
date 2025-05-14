## Hillaire's benchmark on instancing

Hillaire, in his `Improving Performance by Reducing Calls to the Driver` article in [*OpenGL Insights*](http://www.openglinsights.com/) presented results from his benchmark on efficiency of instancing, in which he compared OpenGL's instancing methods. While insightful at the time, the article now shows its age and contains several limitations in the context of modern OpenGL practices—for instance, the use of uniforms to transmit instance data. 

At the time of Hillaire's writing, only the uniform storage was available - and thus being restricted to a certain, relatively low data limit. Nowadays, modern OpenGL applications utilize *SSBO*s freely, which provide an efficient way of transporting data, unrestricted to allocate up to possibly whole available GPGPU memory. Additionally, this benchmark had also pointed out a certain **breaking point**, where noninstanced method performed better than those instanced. This benchmark needed a little refresh in modern context. 

As part of my diploma thesis, I have reevaluated Hillaire's benchmark with modern techniques. The primary change to Hillaire's described approach are *SSBO* in places where Hillaire used uniforms. The aditional change is that benchmarking is done on significantly larger data, as the volumes Hillaire has worked with prove no informative value with current hardware capabilities.

## Exection
The benchmark builds 9 different binaries using `CMakeLists.txt`, in which different compiler flags are used. 3 approaches are tested: 
- noninstanced, 
- instanced with *SSBO*s 
- instanced with *Vertex Attributes*
 
For each approach, 3 different sizes of data are to be tested: 1, 2 or 3 `vec3` elements per vertex.

The code provides multiple values to be redefined:

| `#define` flag | values | default value | note |
|--|--|--|
|`D_REPEATS` | `int` | `10`  | defines how many times each data-size will be repeated |
|`D_INSTANCED` | `int<0, 1, 2>` | `0` |  `0`: noninstanced, `1`: vertex attribute instanced, `2`: SSBO instanved |
|`D_VEC` | `int[0, 2]`| `1` | selects the number ov `vec3` sent per vertex|
|`D_TEST_TIME` | `int` |`3` | the time it takes one data-size to execute |
|`D_ALLOCATE_PERSEC` | `int` | `5000` | internal variable deciding on the fixed size of the buffer that keeps the evaluated values of tests. defines how many values are at most expected per frame.  |
|`D_INIT_SIZE` | `int` | `1048576` | defines the starting amount of vertices from which the tests will grow up to the `D_DEST_SIZE`|
|`D_DEST_SIZE` | `int`| `4194304` |  desired destination amount of vertices |

The results are available being printed into console. 


---

Use with care and love.

----
DT-2025, FAS UWB, axdrdak@students.zcu.cz, 