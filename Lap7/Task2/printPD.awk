#! /usr/bin/awk -f
BEGIN{FS=","
    count=0
}
{
    if(NR!=1&&$3=="bachelor's degree"){
       count++
    }
}
END{
    print "the Students't NUmber: " count
}