#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub struct Slice {
	start: isize,
	count: usize,
	step: isize,
}

#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub enum DynamicSubscript {
	Ellipsis,
	NewAxis,
	Index(isize),
	Slice(Slice),
}

pub fn ellipsis() -> DynamicSubscript {
	DynamicSubscript::Ellipsis
}

pub fn new_axis() -> DynamicSubscript {
	DynamicSubscript::NewAxis
}

pub fn end() -> usize {
	usize::MAX
}

pub fn all() -> Slice {
	Slice::new(0, end(), 1)
}

pub fn even() -> Slice {
	Slice::new(0, end(), 2)
}

pub fn odd() -> Slice {
	Slice::new(1, end(), 2)
}

pub fn make_slice(count: usize) -> Slice {
	Slice::new(0, count, 1)
}

pub fn make_slice_start(start: isize, count: usize) -> Slice {
	Slice::new(start, count, 1)
}

pub fn make_slice_full(start: isize, count: usize, step: isize) -> Slice {
	Slice::new(start, count, step)
}

impl Slice {
	pub fn new(start: isize, count: usize, step: isize) -> Self {
		Self { start, count, step }
	}

	pub fn start(self) -> isize {
		self.start
	}

	pub fn count(self) -> usize {
		self.count
	}

	pub fn step(self) -> isize {
		self.step
	}
}

pub fn sanitize_slice(slice: Slice, extent: usize) -> Result<Slice, String> {
	sanitize_slice_step(slice)?;
	let slice = sanitize_slice_start(slice, extent)?;
	sanitize_slice_count(slice, extent)
}

fn sanitize_slice_step(slice: Slice) -> Result<(), String> {
	if slice.step == 0 {
		return Err("Slice step cannot be zero.".to_string());
	}

	Ok(())
}

fn sanitize_slice_start(mut slice: Slice, extent: usize) -> Result<Slice, String> {
	let start = slice.start;
	let count = slice.count;
	let step = slice.step;

	if start < -(extent as isize) {
		return Err(format!(
			"Slice's negative start index {} is out of bounds for extent {}.",
			start, extent
		));
	}

	if step > 0 || count == 0 {
		if start > extent as isize {
			return Err(format!(
				"Slice's start index {} is out of bounds for extent {}.",
				start, extent
			));
		}
	} else if start >= extent as isize {
		return Err(format!(
			"Backwards slice's start index {} is out of bounds for extent {}.",
			start, extent
		));
	}

	if start < 0 {
		slice.start = start + extent as isize;
	}

	Ok(slice)
}

fn sanitize_slice_count(mut slice: Slice, extent: usize) -> Result<Slice, String> {
	let start = slice.start;
	let count = slice.count;
	let step = slice.step;

	if step > 0 {
		if count == end() {
			slice.count = (extent - start as usize).div_ceil(step as usize);
		} else if count > 0 && start + step * (count as isize - 1) >= extent as isize {
			return Err(format!(
				"Slice count {} start index {} and step {} overflows extent {}",
				count, start, step, extent
			));
		}
	} else {
		let abs_step = -step;
		if count == end() {
			slice.count = start as usize / abs_step as usize + 1;
		} else if count > 0 && abs_step * (count as isize - 1) > start {
			return Err(format!(
				"Reversed slice with count {} start index {} and step {} underflows 0",
				count, start, step
			));
		}
	}

	Ok(slice)
}
