<?php

namespace AppBundle\Entity;

use Doctrine\ORM\Mapping as ORM;

/**
 * @ORM\Entity
 * @ORM\Table(name="channel_intensity")
 */
class ChannelIntensity
{
    /**
     * @ORM\Column(type="integer")
     * @ORM\Id
     * @ORM\GeneratedValues(strategy="AUTO")
     *
     * @var integer
     */
    private $channel;

    /**
     * @ORM\Column(type="integer")
     *
     * @var integer
     */
    private $intensity;


    /**
     * Get channel
     *
     * @return integer 
     */
    public function getChannel()
    {
        return $this->channel;
    }

    /**
     * Set intensity
     *
     * @param integer $intensity
     * @return ChannelIntensity
     */
    public function setIntensity($intensity)
    {
        $this->intensity = $intensity;

        return $this;
    }

    /**
     * Get intensity
     *
     * @return integer 
     */
    public function getIntensity()
    {
        return $this->intensity;
    }
}
