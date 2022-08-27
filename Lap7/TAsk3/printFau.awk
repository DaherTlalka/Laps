#! /usr/bin/awk -f
BEGIN{FS=","
    count=0
}
{
    if(NR!=1&&((index($0,"fan")!=0)||(index($0,"fun")!=0)))
        count++
}
END{
    print "The number of poem that have fun or fan is  :" count
}