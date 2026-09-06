MRC test assets

Both files come from the EMDB and are here so that the MRC reader is tested
against bytes this project did not write. Everything the reader and the writer
can produce themselves is built inside the tests instead, where the bytes sit
next to the assertion.

They were found by the mrcfile project, which scanned the EMDB FTP server for
the smallest map and the smallest map with a space group above one, and which
redistributes them under a BSD licence. There is no smaller specimen of either
property to be had.


EMD-3197.map
33,024 bytes
Sub-tomogram average of a vesicle-forming membrane, space group 1.
https://www.ebi.ac.uk/pdbe/entry/emdb/EMD-3197

Carries nversion = 0 and the machine stamp 0x44 0x41, both of which predate
MRC2014. A reader that gated on either would refuse a file the ecosystem
considers ordinary, so this file is what pins that neither is validated.


EMD-3001.map
315,084 bytes
MicroED structure of a peptide from alpha-synuclein, space group 4 (P21).
https://www.ebi.ac.uk/pdbe/entry/emdb/EMD-3001

Its grid sampling (40, 12, 72) has nothing to do with its extents
(73, 43, 25): in a crystallographic file MZ is a unit cell sampling and not a
count of sections per volume. It also carries a 160 byte extended header, so
the offset its values begin at is one this project did not lay out.
