module permanent
use mykind
use gen_subsets
integer,parameter::changeSize=8
!prestore the traval ordet to speed up
integer,dimension(3,2**(changeSize-1)-1)::order_
contains
  subroutine generate_traval_order
    implicit none
    integer i
    logical end

    call initialize_sub(changeSize-1)
    i=1
    do 
       call next_sub(order_(1,i),order_(2,i),order_(3,i),end)
       i=i+1
       if(end) exit
    end do
  end subroutine generate_traval_order
  
  subroutine printMatrix(A)
    implicit none
    integer(matrix_type),dimension(:,:)::A
    integer r
    do r=1,size(A,1)
       write (*,*) A(r,:)
    end do
  end subroutine printMatrix
  
  subroutine load(m,path)
    implicit none
    integer(matrix_type),dimension(:,:),allocatable::m
    character(len=*)::path
    character(len=100)::buffer
    integer(matrix_type) :: e
    integer r,c,i,rows,cols
    integer :: start=1,off
    open(unit=10,file=path)
    read(10,'(A)') buffer
    if(index(buffer,'Octave ')==0) start=0
    read (10,*)
    read (10,*)
    read (10,*)
    read(10,'(A)') buffer
    off=index(buffer,':')
    read(buffer(off+1:),*) rows
    read(10,'(A)') buffer
    off=index(buffer,':')
    read(buffer(off+1:),*) cols

    allocate(m(rows,cols))
    m=0

    do 
       read (10,*,end=2) r,c,e
       m(r+1-start,c+1-start)=e
    end do
2   close(10)
  end subroutine load
  subroutine minor(A,r,c,A1)
    implicit none
    integer(matrix_type),dimension(:,:)::A
    integer(matrix_type),dimension(:,:)::A1
    integer r,c
    A1(1:r-1,1:c-1)=A(1:r-1,1:c-1)
    A1(1:r-1,c:size(A1,2))=A(1:r-1,c+1:size(A,2))
    A1(r:size(A1,1),1:c-1)=A(r+1:size(A,1),1:c-1)
    A1(r:size(A1,1),c:size(A1,2))=A(r+1:size(A,1),c+1:size(A,2))
  end subroutine minor

  recursive integer(matrix_type) function IDEM(A) result(ret)
    implicit none
    integer(matrix_type),dimension(:,:)::A
    logical,dimension(size(A,1),size(A,2))::nonzeros
    integer(matrix_type),dimension(size(A,1)-1,size(A,2)-1)::A1
    integer::rowSize(size(A,1)),colSize(size(A,2))
    integer minRow,minCol
    integer minRowSize,minColSize
    integer,dimension(:),allocatable::indices
    integer i,r,c,c1,c2,r1,r2
    ret=0
#ifdef debug
       call printMatrix(A)
#endif
    if(size(A,1)==3) then
       ret=A(1,1)*(A(2,2)*A(3,3)+A(2,3)*A(3,2))&
            + A(1,2)*(A(2,1)*A(3,3)+A(2,3)*A(3,1))&
            + A(1,3)*(A(2,1)*A(3,2)+A(2,2)*A(3,1))
       return
    end if
    nonzeros=(A/=0)
    rowSize=count(nonzeros,2)
    minRow=minloc(rowSize,dim=1)!dim is used to get a scalar result
    minRowSize=rowSize(minRow)
    colSize=count(nonzeros,1)
    minCol=minloc(colSize,dim=1)!dim is used to get a scalar result
    minColSize=colSize(minCol)
    if ( minRowSize<=minColSize ) then
#ifdef debug
       write (*,*) 'minRowSize',minRowSize
#endif
       if ( minRowSize==0 ) then
          return
       end if
       allocate(indices(minRowSize))
       c=1
       do i=1,minRowSize
          do while(.not.nonzeros(minRow,c))
             c=c+1
          end do
          indices(i)=c
          c=c+1
       end do
#ifdef debug
       write (*,*) 'choose row ',minRow
       write (*,*) 'cols: ',indices
#endif
       do i=1,minRowSize-1,2
          c1=indices(i)
          c2=indices(i+1)
          call minor(A,minRow,c2,A1)
          A1(1:minRow-1,c1)=A(minRow,c2)*A1(1:minRow-1,c1)+A(minRow,c1)*A(1:minRow-1,c2)
          A1(minRow:size(A1,1),c1)=A(minRow,c2)*A1(minRow:size(A1,1),c1)+A(minRow,c1)*A(minRow+1:size(A,1),c2)
          ret=ret+IDEM(A1)
       end do
       if ( mod(minRowSize,2)==1 ) then
#ifdef debug
          write (*,*) "NEW BRANCH"
#endif
          c1=indices(minRowSize)
          call minor(A,minRow,c1,A1)
          ret=ret+A(minRow,c1)*IDEM(A1)
       end if
       deallocate(indices)
    else
#ifdef debug
       write (*,*) 'minColSize',minColSize
#endif
       if ( minColSize==0 ) then
          return
       end if
       allocate(indices(minColSize))
       r=1
       do i=1,minColSize
          do while(.not.nonzeros(r,minCol))
             r=r+1
          end do
          indices(i)=r
          r=r+1
       end do
#ifdef debug
       write (*,*) 'choose col ',minCol
       write (*,*) 'rows: ',indices
#endif
       do i=1,minColSize-1,2
          r1=indices(i)
          r2=indices(i+1)
          call minor(A,r2,minCol,A1)
          A1(r1,1:minCol-1)=A(r2,minCol)*A1(r1,1:minCol-1)+A(r1,minCol)*A(r2,1:minCol-1)
          A1(r1,minCol:size(A1,2))=A(r2,minCol)*A1(r1,minCol:size(A1,2))+A(r1,minCol)*A(r2,minCol+1:size(A,2))
          ret=ret+IDEM(A1)
       end do
       if ( mod(minColSize,2)==1 ) then
#ifdef debug
          write (*,*) "NEW BRANCH"
#endif
          r1=indices(minColSize)
          call minor(A,r1,minCol,A1)
          ret=ret+A(r1,minCol)*IDEM(A1)
       end if
       deallocate(indices)
    end if
  end function IDEM
  integer(matrix_type) function RNW(A)
    implicit none
    integer(matrix_type),dimension(:,:)::A
    integer(matrix_type),dimension(size(A,1))::S
    !for generating next subset
    integer::next,add_or_remove,k
    logical::end
    S=-sum(A,2)+2*A(:,size(A,2))
    RNW=product(S,1)
    call initialize_sub(size(A,1)-1)
    do 
       call next_sub(next,add_or_remove,k,end)
       S=S+2*add_or_remove*A(:,next)
       if (mod(k,2)==0) then
          RNW=RNW+product(S,1)
       else
          RNW=RNW-product(S,1)
       end if
       if(end) exit
    end do
    RNW=RNW/2**(size(A,1)-1)
    if(mod(size(A,1),2)==0) RNW=-RNW
  end function RNW
  integer(matrix_type) function RNW_presave(A)
    implicit none
    integer(matrix_type),dimension(changeSize,changeSize)::A
    integer(matrix_type),dimension(changeSize)::S
    integer i
    S=-sum(A,2)+2*A(:,changeSize)
    RNW_presave=product(S,1)
    do i=1,size(order_,2)
       S=S+2*order_(2,i)*A(:,order_(1,i))
       if (mod(order_(3,i),2)==0) then
          RNW_presave=RNW_presave+product(S,1)
       else
          RNW_presave=RNW_presave-product(S,1)
       end if
    end do
    RNW_presave=RNW_presave/2**(size(A,1)-1)
    if(mod(size(A,1),2)==0) RNW_presave=-RNW_presave
  end function RNW_presave
  recursive integer(matrix_type) function H(A) result(ret)
    implicit none
    integer(matrix_type),dimension(:,:)::A
    logical,dimension(size(A,1),size(A,2))::nonzeros
    integer(matrix_type),dimension(size(A,1)-1,size(A,2)-1)::A1
    integer::rowSize(size(A,1)),colSize(size(A,2))
    integer minRow,minCol
    integer minRowSize,minColSize
    integer,dimension(:),allocatable::indices
    integer i,r,c,c1,c2,r1,r2
    ret=0
#ifdef debug
       call printMatrix(A)
#endif
    if(size(A,1)==changeSize) then
       ret=RNW_presave(A)
       return
    elseif (size(A,1)<changeSize) then
       ret=RNW(A)
       return
    end if
    nonzeros=(A/=0)
    rowSize=count(nonzeros,2)
    minRow=minloc(rowSize,dim=1)!dim is used to get a scalar result
    minRowSize=rowSize(minRow)
    colSize=count(nonzeros,1)
    minCol=minloc(colSize,dim=1)!dim is used to get a scalar result
    minColSize=colSize(minCol)
    if ( minRowSize<=minColSize ) then
#ifdef debug
       write (*,*) 'minRowSize',minRowSize
#endif
       if ( minRowSize==0 ) then
          return
       end if
       allocate(indices(minRowSize))
       c=1
       do i=1,minRowSize
          do while(.not.nonzeros(minRow,c))
             c=c+1
          end do
          indices(i)=c
          c=c+1
       end do
#ifdef debug
       write (*,*) 'choose row ',minRow
       write (*,*) 'cols: ',indices
#endif
       do i=1,minRowSize-1,2
          c1=indices(i)
          c2=indices(i+1)
          call minor(A,minRow,c2,A1)
          A1(1:minRow-1,c1)=A(minRow,c2)*A1(1:minRow-1,c1)+A(minRow,c1)*A(1:minRow-1,c2)
          A1(minRow:size(A1,1),c1)=A(minRow,c2)*A1(minRow:size(A1,1),c1)+A(minRow,c1)*A(minRow+1:size(A,1),c2)
          ret=ret+H(A1)
       end do
       if ( mod(minRowSize,2)==1 ) then
#ifdef debug
          write (*,*) "NEW BRANCH"
#endif
          c1=indices(minRowSize)
          call minor(A,minRow,c1,A1)
          ret=ret+A(minRow,c1)*H(A1)
       end if
       deallocate(indices)
    else
#ifdef debug
       write (*,*) 'minColSize',minColSize
#endif
       if ( minColSize==0 ) then
          return
       end if
       allocate(indices(minColSize))
       r=1
       do i=1,minColSize
          do while(.not.nonzeros(r,minCol))
             r=r+1
          end do
          indices(i)=r
          r=r+1
       end do
#ifdef debug
       write (*,*) 'choose col ',minCol
       write (*,*) 'rows: ',indices
#endif
       do i=1,minColSize-1,2
          r1=indices(i)
          r2=indices(i+1)
          call minor(A,r2,minCol,A1)
          A1(r1,1:minCol-1)=A(r2,minCol)*A1(r1,1:minCol-1)+A(r1,minCol)*A(r2,1:minCol-1)
          A1(r1,minCol:size(A1,2))=A(r2,minCol)*A1(r1,minCol:size(A1,2))+A(r1,minCol)*A(r2,minCol+1:size(A,2))
          ret=ret+H(A1)
       end do
       if ( mod(minColSize,2)==1 ) then
#ifdef debug
          write (*,*) "NEW BRANCH"
#endif
          r1=indices(minColSize)
          call minor(A,r1,minCol,A1)
          ret=ret+A(r1,minCol)*H(A1)
       end if
       deallocate(indices)
    end if
  end function H
end module permanent
