#! /usr/bin/awk -f
BEGIN{FS=","
    print "Actor Name \t\t\tage\t\t\tYear"
}
{
    if(NR!=1)
        print $4 "\t\t\t" $3 "\t\t\t" $2
}