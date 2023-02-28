    include exec/resident.i
    include lvo/exec_lib.i
            jmp _main
Romtag:     dc.w $4AFC
.rt_match   dc.l Romtag
.rt_endskip dc.l EndSkip
.rt_flags   dc.b RTF_COLDSTART
.rt_version dc.b 41
.rt_type    dc.b 0
.rt_pri     dc.b 0
.rt_name    dc.l Name
.rt_id      dc.l Name
.rt_init    dc.l init

Name: dc.b "Mounter",0

init:       movem.l d2-d7/a2-a6,-(sp)
            lea     EndSkip(PC),a1
            move.l  #$1000,d3            ; Search the first 4096 words for a Romtag
.findrt     cmpi.w  #$4AFC,(a1)
            bne.s   .not_rt
            cmp.l   RT_MATCHTAG(a1),a1   ; Check that the RT_MATCHTAG is correct
            beq.s   .found
.not_rt     addq.l  #2,a1
            dbra    d3,.findrt
            bra.s   .err

* Found the driver, initialize it & mount devices
.found  
            moveq.l  #0,d1
            jsr      _LVOInitResident(A6)
            tst.l    d0
            beq.s    .done
            move.l   d0,-(sp)
            jsr      _main
            move.l   (sp)+,d0
.done
            movem.l (sp)+,d2-d7/a2-a6
            rts
.err
            moveq.l #0,d0
            movem.l (sp)+,d2-d7/a2-a6
            rts