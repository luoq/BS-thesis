module gen_subsets
implicit none
logical,dimension(:),allocatable,save,private::have_
integer,save,private :: k_
contains
  subroutine initialize_sub(n)
    implicit none
    integer :: n
    allocate(have_(n))
    have_=.false.
    k_=0
  end subroutine initialize_sub
  subroutine next_sub(next,addOrRemove,k,end)
    implicit none
    integer :: next,addOrRemove,k
    logical :: end
    next=1
    if ( mod(k_,2)==1 ) then
       do while(.not.have_(next))
          next=next+1
       end do
       next=next+1
    end if
    if(have_(next)) then
       addOrRemove=-1
       k_=k_-1
    else
       addOrRemove=1
       k_=k_+1
    end if
    have_(next)=.not.have_(next)
    k=k_
    end=.false.
    if ( k_==1.and.have_(size(have_,1)) ) then
       end=.true.
       deallocate(have_)
    end if
  end subroutine next_sub
end module gen_subsets
module timer
implicit none
real(8),save,private::start_,end_
contains
  subroutine tic
    implicit none
    call wtime(start_)
  end subroutine tic
  subroutine toc(time)
    implicit none
    real(8) :: time
    call wtime(end_)
    time=end_-start_
  end subroutine toc
end module timer
module mykind
implicit none
integer,parameter::matrix_type=4
end module mykind
