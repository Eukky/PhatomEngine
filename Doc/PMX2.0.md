# PMX 2.0 file format #

> https://gist.github.com/ulrikdamm/8274171

This is a description of the PMX file format. This is used for 3D models in Miku Miku Dance (MMD). 

Since I couldn't find any English descriptions of the PMX file format, I've made this, which is translated from http://gulshan-i-raz.geo.jp/labs/2012/10/17/pmx-format1/. I haven't used this file format yet, so please don't ask me what everything means.
An English guide to the PMD file format, which preceeded PMX, can be found here: http://mikumikudance.wikia.com/wiki/MMD:Polygon_Model_Data.

If you want to learn more, there are some open source projects on GitHub which can read this format, so go take a look at them.

Note: fields with type text begins with an int (32 bit) with how many bytes of text the section is.
The encoding of the text is either UTF-8 or UTF-16, as specified by the Text Encoding byte in the header.

```
Field                   | bytes | type      | Notes
------------------------+-------+-----------+--------------------------------------------
     -- Header --       |       |           |
                        |       |           |
Signature               | 4     | string    | "PMX "
Version                 | 4     | float     | 2.0
Length                  | 1     | char      | 8
Text Encoding           | 1     | char      | 0 = UTF-16, 1 = UTF-8
Appendix UV             | 1     | char      | 0-4
Vertex Index Size       | 1     | char      | 1 = byte, 2 = short, 4 = int
Texture Index Size      | 1     | char      | 1 = byte, 2 = short, 4 = int
Material Index Size     | 1     | char      | 1 = byte, 2 = short, 4 = int
Bone Index Size         | 1     | char      | 1 = byte, 2 = short, 4 = int
Morph Index Size        | 1     | char      | 1 = byte, 2 = short, 4 = int
Rigid Body Index Size   | 1     | char      | 1 = byte, 2 = short, 4 = int
                        |       |           |
   --  Model info --    |       |           |
                        |       |           |
Character Name          | x     | text      | 
English Character Name  | x     | text      | 
Comment                 | x     | text      | 
English Comment         | x     | text      | 
                        |       |           |
   -- Vertex Data --    |       |           |
                        |       |           |
Vertex Count            | 4     | int       | 
Each Vertex:            |       |           |
- XYZ Position          | 4 * 3 | float     |
- UV Texture Coordinate | 4 * 2 | float     |
- Appendix UV (XYZW)    | 4 * x | float     | Number of elements specified in the Appendix UV header field.
- Weight Type           | 1     | char      | 0 = BDEF1, 1 = BDEF2, 2 = BDEF4, 4 = SDEF.
- If BDEF1 Weight Type: |       |           |
- - Bone 1 Index        | x     | x         | Type specified in the Bone Index Size header field. Bone weight is 1.0.
- If BDEF2 Weight Type: |       |           |
- - Bone 1 Index        | x     | x         | Type specified in the Bone Index Size header field.
- - Bone 2 Index        | x     | x         | Type specified in the Bone Index Size header field.
- - Weight 1            | 4     | float     | Weight 2 = 1.0 - Weight 1
- If BDEF4 Weight Type: |       |           |
- - Bone 1 Index        | x     | x         | Type specified in the Bone Index Size header field.
- - Bone 2 Index        | x     | x         | Type specified in the Bone Index Size header field.
- - Bone 3 Index        | x     | x         | Type specified in the Bone Index Size header field.
- - Bone 4 Index        | x     | x         | Type specified in the Bone Index Size header field.
- - Weight 1            | 4     | float     | 
- - Weight 2            | 4     | float     | 
- - Weight 3            | 4     | float     | 
- - Weight 4            | 4     | float     | 
- If SDEF Weight Type:  |       |           |
- - Bone 1 Index        | x     | x         | Type specified in the Bone Index Size header field.
- - Bone 2 Index        | x     | x         | Type specified in the Bone Index Size header field.
- - Weight 1            | 4     | float     | Weight 2 = 1.0 - Weight 1
- - C (XYZ)             | 4 * 3 | float     | 
- - R0 (XYZ)            | 4 * 3 | float     | 
- - R1 (XYZ)            | 4 * 3 | float     | 
- Edge Scale            | 4     | float     |
                        |       |           |
    -- Face Data --     |       |           |
                        |       |           |
Face Count              | 4     | int       | 
Each Face:              |       |           |
- Vertex Index          | x     | x         | Type specified in Vertex Index Size header field.
                        |       |           |
  -- Texture Data --    |       |           |
                        |       |           |
Texture Count           | 4     | int       |
Each Texture:           |       |           |
- File Name             | x     | text      | 
                        |       |           |
  -- Material Data --   |       |           |
                        |       |           |
Material Count          | 4     | int       |
Each Material:          |       |           |
- Name                  | x     | text      |
- English Name          | x     | text      |
- Diffuse Color (RGBA)  | 4 * 4 | float     | 
- Specular Color (RGB)  | 4 * 3 | float     |
- Specularity           | 4     | float     |
- Ambient Color (RGB)   | 4 * 3 | float     |
- Drawing Mode Flag     | 1     | char      | 0x01 = Double-Sided
                                |           | 0x02 = Shadow
                                |           | 0x04 = Self shadow map
                                |           | 0x08 = Self shadow
                                |           | 0x10 = Draw edges
- Edge Color (RGB)      | 4 * 3 | float     | 
- Edge Size             | 4     | float     |
- Texture Index         | x     | x         | Type specified in the Texture Index Size header field.
- Sphere Index          | x     | x         | Type specified in the Texture Index Size header field.
- Sphere Mode           | 1     | char      | 
- Toon Flag             | 1     | char      |
- Toon Index            | x     | x         | If Toon Flag is 0, then it has the type specified in the Texture Index Size header field.
                        |       |           | If Toon Flag is 1, the size is 1 byte with a value from 0-9.
- Memo                  | x     | text      |
- From Face Number      | 4     | int       |
```