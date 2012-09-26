#!/bin/sh

FILE="paralines.pcv"
echo "*** Testing no filters"
pcv -Tpngcairo -Wdebug -o AAAnofilters.png $FILE
echo "*** Testing equal 175 on axis 1"
pcv -Tpngcairo -Wdebug -o BBBequal175.png $FILE 'show plot = 175 on axis 1'
pcv -Tpngcairo -Wdebug -o BBBequal175-2.png $FILE 'hide plot != 175 on axis 1'
echo "*** Testing less on axis 1"
pcv -Tpngcairo -Wdebug -o CCCless175.png $FILE 'show plot < 175 on axis 1'
pcv -Tpngcairo -Wdebug -o CCCless175-2.png $FILE 'hide plot >= 175 on axis 1'
echo "*** Testing greater on axis 1"
pcv -Tpngcairo -Wdebug -o DDDgreater175.png $FILE 'show plot > 175 on axis 1'
pcv -Tpngcairo -Wdebug -o DDDgreater175-2.png $FILE 'hide plot <= 175 on axis 1'
echo "*** Testing greater equal on axis 1"
pcv -Tpngcairo -Wdebug -o EEEgreaterequal175.png $FILE 'show plot >= 175 on axis 1'
pcv -Tpngcairo -Wdebug -o EEEgreaterequal175-2.png $FILE 'hide plot < 175 on axis 1'
echo "*** Testing less equal on axis 1"
pcv -Tpngcairo -Wdebug -o FFFlessequal175.png $FILE 'show plot <= 175 on axis 1'
pcv -Tpngcairo -Wdebug -o FFFlessequal175-2.png $FILE 'hide plot > 175 on axis 1'

