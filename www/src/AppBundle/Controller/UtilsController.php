<?php

namespace AppBundle\Controller;

use Symfony\Bundle\FrameworkBundle\Controller\Controller;
use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
use Symfony\Component\HttpFoundation\Response;

use AppBundle\Model\DataQueries;

class UtilsController extends Controller
{ 
    /**
     * @Route("/add_patch", name="add_patch")
     */
    public function addAction()
    {
        // TODO
        return NULL;
    }
    
    /**
     * @Route("/delete_patch/{count}", name="delete_patch")
     */
    public function deleteAction($count)
    {
        if (!DataQueries::deletePatch($count)) {
            echo "Something went wrong";
        }
        
        return $this->redirectToRoute('patch');
    }
    
    /**
     * @Route("/submit_values", name="submit_values")
     */
    public function submitAction()
    {
        // TODO
        return NULL;
    }
}
