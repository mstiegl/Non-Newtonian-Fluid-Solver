#
#  Copyright 1995-2010 by IRSN
#
#  This software is an application framework, with a set of integrated
#  reusable components, whose purpose is to simplify the task of developing
#  softwares of numerical mathematics and scientific computing.
#
#  This software is governed by the CeCILL-C license under French law and
#  abiding by the rules of distribution of free software. You can use, modify
#  and/or redistribute the software under the terms of the CeCILL-C license
#  as circulated by CEA, CNRS and INRIA at the following URL
#  "http://www.cecill.info".
#
#  As a counterpart to the access to the source code and rights to copy,
#  modify and redistribute granted by the license, users are provided only
#  with a limited warranty and the software's author, the holder of the
#  economic rights, and the successive licensors have only limited liability.
#
#  In this respect, the user's attention is drawn to the risks associated
#  with loading, using, modifying and/or developing or reproducing the
#  software by the user in light of its specific status of free software,
#  that may mean that it is complicated to manipulate, and that also
#  therefore means that it is reserved for developers and experienced
#  professionals having in-depth computer knowledge. Users are therefore
#  encouraged to load and test the software's suitability as regards their
#  requirements in conditions enabling the security of their systems and/or
#  data to be ensured and, more generally, to use and operate it in the same
#  conditions as regards security.
#
#  The fact that you are presently reading this means that you have had
#  knowledge of the CeCILL-C license and that you accept its terms.
#

##
# MAIN

package main ;
use strict ;
use Getopt::Long;
use File::Find ;
use Pod::Usage;
use Util ;
use Defs ;
use Arch ;

#-------------------------------------------------------------------------
sub output_message {
  my ( $resu, $np, $mesg ) = @_ ;

  print STDOUT $mesg ;
  if( ! $np ) {
    open OUT, ">>$resu" ;
    print OUT $mesg;
    close OUT;
  }
}

#-------------------------------------------------------------------------
sub do_one_run {
  my ( $exe, $data, $resu, $noverb, $np, $mf, $nolocal, @options ) = @_ ;

  my $ret = 1 ;
  if( !$np ) {
     open OUT, ">$resu" ;
     if( ! $noverb ) {
        print OUT "\n" . '#' x 60 ."\n" ;
        print OUT "operating system (result of the command \"uname -a\"):\n";
        my ($sysname, $realhostname, $release) = POSIX::uname();
        use POSIX qw(strftime);
        my $date = strftime "%A %B %Y %H:%M:%S", localtime;
        print OUT "$sysname $realhostname $release $date\n" ;
#	print OUT `uname -a` ;
        print OUT '#' x 60 ."\n" ;
     }

     my $cmd = "$exe @options $data 2>&1" ;
     my $pid = 0 ; # tee not avaliable in Windows...
     if( Arch::is_posix() == 1 ) {
       $pid = open(KID_TO_READ, "-|");
     }
	 
     if ($pid) {
       while (<KID_TO_READ>) {
         print STDOUT $_ ;
         print OUT $_ ;
       }
       close(KID_TO_READ);
       $ret = $? >> 8 ;
     }
     else {
       exec( $cmd ) || Defs::Error "can't exec program: $!";
     }
     close OUT;
  }
  else {
     my $cmd = "mpirun " ;
     my @script = Defs::script( "arch" ) ;
     my $cmd_pel_line = "@script -getvariable_extra MPIRUN" ;
     my $cmd_pel = `$cmd_pel_line` ;
     if( -x "$cmd_pel" ) { $cmd="$cmd_pel " ; }
     if( $mf )  { $cmd .= " -machinefile $mf " ; }
     $cmd .= " -np $np " ;
     if( $nolocal )  { $cmd .= " -nolocal " ; }
     $cmd .= " $exe @options -notify_parallel $data" ;
     if( $resu ) { $cmd .= " -o $resu" ; }
     $ret = system( $cmd ) ;
  }
  return( $ret ) ;
}

#-------------------------------------------------------------------------
# MAIN start here
#-------------------------------------------------------------------------
my $noverb = '' ;
my $H = '' ;
my $man = '' ;
my @extra=() ;
my @directories=() ;
my $np = '' ;
my $mf = '' ;
my $nolocal = '' ;
my $build_pattern = undef ;
my $check_pattern = undef ;
my $autocheck = 1;

my $result = GetOptions (
   'man'   => \$man,
   'help'  => \$H,
   'Cpost' => sub { push @extra, "-Cpost" ; },
   'Call'  => sub { push @extra, "-Call" ; },
   'noverb' => \$noverb,
   'Cobjects' => sub { push @extra, "-Cobjects" ; },
   'catch=i' => sub {  my ($c,$i)=@_ ; push @extra, "-catch $i" ; },
   'build_pattern=s' => \$build_pattern,
   'check_pattern=s' => \$check_pattern,
   'autocheck!', \$autocheck, 
   'X=s' => sub {  my ($c,$s)=@_ ; push @extra, "$s" ; },
   'Xpetsc=s' => sub {  my ($c,$s)=@_ ; push @extra, "-Xpetsc $s" ; },
   'np=i' => \$np,
   'machinefile=s' => \$mf,
   'nolocal'=> \$nolocal,
   'R=s' => \@directories ) ;

pod2usage(-exitstatus => 0, -verbose => 2) if $man;
pod2usage( -verbose => 1 ) if $H  ;
if( $mf && !$np ) {
  Defs::Error( "Can't specify machinefile without process number np" ) ;
}
if( $mf && ! -r $mf ) { Defs::Error( "No valid machinefile : $mf" ) } ;
if( $nolocal && !$np ) {
  Defs::Error( "Can't specify nolocal without process number np" ) ;
}
if( $nolocal && !$mf ) {
  Defs::Error( "Can't specify nolocal without machinefile" ) ;
}

if ($build_pattern && $check_pattern) {
  pod2usage('options --build_pattern and --check_pattern are mutually exclusive');
}

if ($build_pattern) {
  push @extra, "-build_pattern $build_pattern";
  $autocheck = 0; #overriden 
}
if ($check_pattern) {
  Defs::Error( "Invalid file: $check_pattern in --check_pattern" ) unless ( -r $check_pattern );
  push @extra, "-check_pattern $check_pattern";
  $autocheck = 0; #overriden 
}

if( scalar @ARGV != 3 || !$result ) {
  pod2usage( -verbose => 0 ) ;
}

if( ! $noverb ) { unshift @extra, "-v" ; }

my ( $exe, $data, $resu ) = @ARGV ;

Defs::Error( "No valid executable : $exe" ) unless ( -X $exe ) ;

my $current_dir = Util::absolute_pathname( File::Spec->curdir() ) ;

my @dirs_with_data = () ;
for( @directories ) {
  if( ! -d $_ ) { Defs::Error( "unknown directory : $_" ) ; }
  File::Find::find( sub { /^$data$/ && 
                          push @dirs_with_data, $File::Find::dir },
                    @directories ) ;
}

if( ! @directories ) {
  if( ! -r $data ) { Defs::Error( "No valid datafile : $data" ) } ;
  push( @dirs_with_data, $current_dir ) ;
}

my $tmp_patternfile = $resu . '.pattern' . $$;
my $ret = 0 ;
for( @dirs_with_data ) {
   chdir( $_ ) || Defs::Error( "cannot chdir : $_" ) ;

   #add the build_pattern option when autocheck is activated
   if ($autocheck) {
     push (@extra, "-build_pattern $tmp_patternfile");
   }

   #normal execution
   my $exe_ret = do_one_run( $exe, $data, $resu, $noverb,
                             $np, $mf, $nolocal, @extra ) ;
   $ret += $exe_ret ;

   #control afterwards when autocheck is activated
   if ($autocheck) {
     if ( ! $exe_ret ) {
       my $mesg = "\n" . '#' x 60 ."\nautocheck of $data\n" ;
       output_message( $resu, $np, $mesg ) ;

       my @checkoptions = ( "-A check -s $tmp_patternfile" ) ;
       my $checknoverb = 1 ;

       #control vs the temporary pattern file
       my $check_resu = ">$resu" ;
       if( $np ) { $check_resu = undef ; }
       my $check_ret = do_one_run( $exe, $data, $check_resu, $checknoverb,
                                   $np, $mf, $nolocal, @checkoptions ) ;
       $ret += $check_ret ;

       $mesg = "" ;
       if( ! $check_ret ) {
         $mesg = "... successful\n";;
       }
       $mesg = $mesg . '#' x 60 . "\n";
       output_message( $resu, $np, $mesg ) ;
     } else {
       my $mesg = "\n\n" . '#' x 60 .
                  "\n    no autocheck performed because" .
                  "\n    the execution ended abnormally\n".
                  '#' x 60 . "\n" ;
       output_message( $resu, $np, $mesg ) ;
     }
     #remove the temporary pattern file
     unlink( $tmp_patternfile );
   }
   chdir( $current_dir ) || Defs::Error( "cannot chdir : $current_dir" ) ;
}
exit( $ret ) ;

##
# POD Documentation
#
__END__

=head1 NAME

run - execution of a PELICANS-based application

=head1 SYNOPSIS

pel run [-help|-man]

pel run [options...] F<exe> F<data> F<resu>

pel run -build_pattern F<filename> -R F<dir> F<exe> F<data> F<resu>

pel run -check_pattern F<filename> F<exe> F<data> F<resu>

=head1 DESCRIPTION

C<pel run> executes a given PELICANS-based application with a
given data file, and copy all the output messages in a given file
(or set of files in parallel mode).

Execution is performed in the current directory unless the option
C<-R> is specified. Other options are devoted to the customization
of the execution.

=head1 ARGUMENTS

=over

=item B< F<exe> >

Name of the executable of the PELICANS-based application to run
(this executable has usually been built with a C<pel build>
command).

=item B< F<data> >

Name of the file storing the PELICANS Hierarchical Data Structure
used as a data file for the PELICANS-based application to run.

=item B< F<resu> >

In sequential mode :
name of a file into which any message directed to the standard
error or to the standard output will be copied (this file will
be created or truncated).
In parallel mode : basename of the files into which any message
directed to the streams C<PEL::out()> and C<PEL::err()> will
be copied (these files will be created or truncated). The extension
of these files is the rank of their associated process.

=back

=head1 OPTIONS

=over

=item B<-h, -help>

Print a brief help message and exit.

=item B<-m, -man>

Print the manual page and exit.

=item B<-noverb>

Deactivate the verbose option of the PELICANS-based application
(which is activated by default).

=item B<-R> F<dir>

Instead of running in the current directory, run recursively in
all the subdirectories of F<dir> (including F<dir> itself)
that contain a file of name F<data>.

=item B<-Cpost>

Activate the evaluation of the postconditions (only effective
for sources compiled with the 'C<opt2>' or 'C<dbg>' compilation level, 
see C<pel depend> ).

=item B<-Call>

Activate the evaluation of the postconditions, invariants and
checks (only effective
for sources compiled with the 'C<opt2>' or 'C<dbg>' compilation level,
see C<pel depend> ).

=item B<-Xpetsc> F<option>

Transfer F<option> to the PETSc external API.

=item B<-X> F<option>

Transfer F<option> to external APIs.

=back

To execute a PELICANS-based application in parallel on
multiple processors, C<pel run> forwards its request
to C<mpirun>.

=over

=item B<-np> n

Specify the number of processors to run on
(call C<mpirun> with B<-np> n as an option).

=item B<-machinefile> F<file>

Take the list of possible machines to run on from
the file F<file> (call C<mpirun> with
B<-machinefile> F<file> as an option).

=item B<-nolocal>

Call C<mpirun> with B<-nolocal> as an option.

=back

During the final stage of the execution of a PELICANS-based application,
the object to which C<PEL_Root::object()> refers is destroyed
(internally in the PELICANS framework), leading to
the destruction of all remaining instances of subclasses of C<PEL_Object>
whose owner is not the C<NULL> object. But it is the reponsibility of
the developer of the PELICANS-based application to call the C<destroy()>
method on behalf of any objects that he may have created with C<NULL>
as owner. If such is not the case, some dynamically allocated memory
might not be released when the execution terminates, and an ad-hoc
message is printed by PELICANS. The following two options
help identifying the functions in which non-destroyed objects
have been created. They should be used during two successive runs:
the first run, with the C<-Cobject> option identifies a non-destroyed
object whereas the second run, with the C<-catch> option locates
the function in which it had been created.

=over

=item B<-Cobjects>

Assign an identification number to each object that
has not been destroyed when the execution terminates,
and display all available information about them
(the identification number is an integer appearing
between brackets on top of each printed block).

=item B<-catch> nb

Display a warning message at runtime when the
object whose identification number is C<nb>
is created (necessarily with the C<NULL> owner).

=back

A PELICANS-based application that can be executed by C<pel run>
requires a data file (whose name is the argument F<data>).
That file stores a Hierarchical Data System whose structure
is implicitely defined by the application itself (through
calls to the various member functions of C<PEL_ModuleExplorer>).
PELICANS offers the possibility to "learn" that structure
during an execution and to record this infered knowledge
in a called "pattern" file (option C<-build_pattern>).
This pattern file can be used in turn to check
the conformity of other data file with this structure
(option C<-check_pattern>).

These two options are mutually exclusive. The activation of
one of them inhibits the autocheck feature (see below).

=over

=item B<-build_pattern> F<filename>

During the run, extract the pattern of the hierarchical
data structure associated to F<data>, and add it to the
file called F<filename>
(which is created if it did not existed before, or extended otherwise).

=item B<-check_pattern> F<filename>

Prior to execution, check the conformity of F<data>
with the pattern file F<filename>.

=back

Let us consider an execution associated to a given data file F<data>. If
there is no available pattern file against which the conformity
of F<data> can be checked, C<pel run> offers an "autocheck"
mode which splits the execution in two steps: a run is first performed
and a pattern is extracted from F<data> (in a temporary file), then, after
completion of that run, F<data> is checked against the extracted
pattern. This mode allows finding unread parts in the data
file which may be caused by typing errors.

=over

=item B<-autocheck>

Perform the run in the "autocheck" mode. This option is activated by default,
unless the options B<-build_pattern> or B<-check_pattern> are present.

=item B<-noautocheck>

Deactivate the "autocheck" mode.

=back

=head1 EXAMPLES

=over

=item C<pel run  ../bin/exe data.pel resu>

Run executable F<exe> located in the directory F<../bin/>
with the data file F<data.pel>, store
all outputs in the file F<resu> and perform an autocheck on F<data.pel>.

=item C<pel run -Cpost ../bin/exe data.pel resu>

Same as before, with in addition the evaluation of all postconditions
of the member functions compiled with the 'C<dbg>' or 'C<opt2>'
compilation level.

=item C<pel run -np 3 -machinefile ms $EXE0 data.pel resu>

Launch the executable whose name is stored in the C<EXEO> environment
variable on 3 processors, with the data file F<data.pel>.
The first process will be on the current machine, whereas the other ones will
be on the machines defined in the file F<ms>).
Outputs directed to C<PEL::out()>
and C<PEL::err()> will be copied in the files F<resu.0>, F<resu.1>
and F<resu.2> (first, second and third process).

=item C<pel run -np 3 -machinefile ms -nolocal $EXE0 data.pel resu>

Same as before, but the 3 processes will be launched on the machines defined
in the file F<ms>.

=item C<pel run -np 3 -machinefile ms -nolocal -Xpetsc -trace $EXE0 data.pel resu>

Same as before, with a transfer of the F<-trace> option to PETSc.

=item C<pel run -R Test bin/exe data.pel resu>

If C<EXE> denotes the file F<exe> located in the subdirectory
F<bin> of the working directory, this command is equivalent as
executing: S< C<pel run EXE data.pel resu> >
in all the subdirectories of F<Test> containing
a file F<data.pel>.

=item C<pel run -R -build_pattern etc/pattern.pel bin/exe data.pel resu>

Same as before, with in addition the learning and storage of the
requested structure of the data files in F<etc/pattern.pel>. 
No autocheck is performed.

=item C<pel run -check_pattern ../etc/pattern.pel ../bin/exe data.pel resu>

Check the conformance of F<data.pel> with F<../etc/pattern.pel>
and, if successful, run subsequently F<../bin/exe> with data file
F<data.pel> and store all outputs in the file F<resu>. No autocheck is
performed.

=back

=cut
