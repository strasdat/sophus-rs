use pyo3::prelude::*;
use sophus_rs::image::IntensityImage;

#[pymodule]
fn pysophus(_py: Python<'_>, m: &PyModule) -> PyResult<()> {
    m.add_class::<IntensityImage>()?;
    m.add_function(wrap_pyfunction!(sophus_rs::image::new_image, m)?)?;
    Ok(())
}
