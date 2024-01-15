.include "macros.inc"

.section .data

glabel _CityscapeSeg4
.incbin "/src/leveldata/cityscape/Segment4.MIO0"
.balign 16
glabel _CityscapeSeg4END

glabel _CityscapeSeg5
.incbin "/src/leveldata/cityscape/Segment5.MIO0"
.balign 16
glabel _CityscapeSeg5END

glabel _CityscapeSeg6
.incbin "/src/leveldata/cityscape/Segment6.MIO0"
.balign 16
glabel _CityscapeSeg6END


glabel _MansionSeg4
.incbin "/src/leveldata/Mansion/Segment4.MIO0"
.balign 16
glabel _MansionSeg4END

glabel _MansionSeg5
.incbin "/src/leveldata/Mansion/Segment5.MIO0"
.balign 16
glabel _MansionSeg5END

glabel _MansionSeg6
.incbin "/src/leveldata/Mansion/Segment6.MIO0"
.balign 16
glabel _MansionSeg6END



glabel _BobombSeg4
.incbin "/src/leveldata/bobomb/Segment4.MIO0"
.balign 16
glabel _BobombSeg4END

glabel _BobombSeg5
.incbin "/src/leveldata/bobomb/Segment5.MIO0"
.balign 16
glabel _BobombSeg5END

glabel _BobombSeg6
.incbin "/src/leveldata/bobomb/Segment6.MIO0"
.balign 16
glabel _BobombSeg6END