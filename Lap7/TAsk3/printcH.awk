#! /usr/bin/awk -f
BEGIN{FS=","
    count=0
}
{
    if(NR!=1&&(index($2,"happy")!=0))
        print $1" "$2"---"$3"---"$4
        count++
}