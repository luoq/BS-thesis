program Per
  use permanent
  use timer
  use mykind
  implicit none
  integer(matrix_type),dimension(:,:),allocatable :: m
  integer(matrix_type)::result
  character(len=100)::path
  real(8) :: time,time1
#ifdef debug
#endif
  call get_command_argument(1,path)
  call load(m,path)
  !write (*,*) IDEM(m)
  !write (*,*) RNW(m)
  
  !call generate_traval_order
  call tic
  result=IDEM2(m)
  call toc(time)

 ! if ( time<1e-1 ) then
 !    do
 !       call tic
 !       result=H(m)
 !       call toc(time1)
 !       if ( time1>=(1+0.0)*time ) then
 !          exit
 !       end if
 !       time=time1
 !    end do
 ! end if
  write (*,*) result,time

  deallocate(m)
end program Per
