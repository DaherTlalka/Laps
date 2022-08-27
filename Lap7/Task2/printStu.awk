#! /usr/bin/awk -f
BEGIN{FS=","
    print "======="
    print "Success Student List"
    print "======="
    count=0
}
{
    if(NR!=1&&$6>=80&&$7>=80&&$8>=80){
        count++
        print $0
        print ""
      
    }
}
END{
    print "The number of students:"count
} 