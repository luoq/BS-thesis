program test_RNW
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
  
  call tic
  result=RNW(m)
  call toc(time)
  write (*,*) result,time

  deallocate(m)
end program test_RNW
