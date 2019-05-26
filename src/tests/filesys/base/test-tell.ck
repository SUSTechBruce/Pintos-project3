# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected (IGNORE_EXIT_CODES => 1, [<<'EOF']);
(test-tell) begin
(test-tell) create "test_file"
(test-tell) open "test_file"
(test-tell) file position is correct
(test-tell) end
EOF
pass;
