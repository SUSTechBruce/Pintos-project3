# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(sc-null-ptr) begin
sc-null-ptr: exit(-1)
EOF
pass;
