#! /usr/bin/awk -f
BEGIN{FS=","
    sumR=0
    sumW=0
    sumM=0
    h=0
}
{
    if(NR!=1&&$4=="standard"){
        h++
       sumM+=$6
       sumR+=$7
       sumW+=$8
    }
}
END{
    sumM=sumM/h
    sumR=sumR/h
    sumW=sumW/h
    print "the avg of math:"sumM
    print "the agv of Read:"sumR
    print "the agv of write:"sumW
}
//if the want every one alone this is the code
if(NR!=1&&$4=="standard"){
        l++
        sum=$6+$7+$8
        sum=sum/3
        print "the average:"sum 
    } 